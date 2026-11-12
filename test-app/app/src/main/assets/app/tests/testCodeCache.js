// Needs "codeCache": true in the test app's package.json. Every load runs in a
// fresh worker, which has its own module registry, so the probe compiles again
// and consults the cache entry the previous load left behind.
describe("V8 code cache", function () {
    var File = java.io.File;
    var HEADER_SIZE = 24;
    var UNTOUCHED_MTIME = 1000000;

    var filesDir = new File(__approot);
    var cacheDir = new File(filesDir.getParentFile(), "code_cache/v8");
    var probeDir = new File(filesDir, "app/codeCacheProbe");

    // Mirrors the runtime's entry name: FNV-1a 64 of the logical path.
    function cacheFileFor(path, isModule) {
        var hash = 0xcbf29ce484222325n;
        var mask = (1n << 64n) - 1n;
        var bytes = new TextEncoder().encode(path);
        for (var i = 0; i < bytes.length; i++) {
            hash ^= BigInt(bytes[i]);
            hash = (hash * 0x100000001b3n) & mask;
        }
        var name = hash.toString(16).padStart(16, "0") + (isModule ? ".mcache" : ".cache");
        return new File(cacheDir, name);
    }

    function writeText(file, text) {
        var writer = new java.io.FileWriter(file);
        writer.write(text);
        writer.close();
    }

    function readBytes(file) {
        var raf = new java.io.RandomAccessFile(file, "r");
        var bytes = Array.create("byte", raf.length());
        raf.readFully(bytes);
        raf.close();
        return bytes;
    }

    function writeByte(file, offset, value) {
        var raf = new java.io.RandomAccessFile(file, "rw");
        raf.seek(offset);
        raf.write(value);
        raf.close();
    }

    function readUint32(bytes, offset) {
        return ((bytes[offset] & 255) |
                ((bytes[offset + 1] & 255) << 8) |
                ((bytes[offset + 2] & 255) << 16) |
                ((bytes[offset + 3] & 255) << 24)) >>> 0;
    }

    function writeUint32(file, offset, value) {
        for (var i = 0; i < 4; i++) {
            writeByte(file, offset + i, (value >>> (8 * i)) & 255);
        }
    }

    function payloadCrc(bytes) {
        var crc = new java.util.zip.CRC32();
        crc.update(bytes, HEADER_SIZE, bytes.length - HEADER_SIZE);
        return Number(crc.getValue());
    }

    function expectValidEntry(file) {
        expect(file.isFile()).toBe(true);
        var bytes = readBytes(file);
        expect(bytes.length).toBeGreaterThan(HEADER_SIZE);
        var magic = String.fromCharCode(bytes[0], bytes[1], bytes[2], bytes[3]);
        expect(magic).toBe("NSCC");
        expect(readUint32(bytes, 16)).toBe(bytes.length - HEADER_SIZE);
        expect(readUint32(bytes, 20)).toBe(payloadCrc(bytes));
        return bytes;
    }

    function deleteTree(file) {
        var children = file.listFiles();
        if (children) {
            for (var i = 0; i < children.length; i++) {
                deleteTree(children[i]);
            }
        }
        file.delete();
    }

    function loadInWorker(probe, callback) {
        var worker = new Worker("./codeCacheLoadWorker.js");
        worker.onmessage = function (msg) {
            worker.terminate();
            callback(msg.data);
        };
        worker.postMessage({ path: probe.path, isModule: probe.isModule });
    }

    [false, true].forEach(function (isModule) {
        describe(isModule ? "for ES modules" : "for CommonJS modules", function () {
            // Created by the first spec and removed by the last: the specs build
            // on each other's cache entry, in order.
            var probe;

            function createProbe() {
                probeDir.mkdirs();
                var token = "probe-" + Date.now() + "-" + Math.floor(Math.random() * 1e9);
                var path = probeDir.getAbsolutePath() + "/" + token + (isModule ? ".mjs" : ".js");
                writeText(new File(path), isModule
                    ? "export const value = '" + token + "';\n"
                    : "module.exports.value = '" + token + "';\n");
                probe = {
                    path: path,
                    isModule: isModule,
                    token: token,
                    cacheFile: cacheFileFor(path, isModule)
                };
            }

            function removeProbe() {
                new File(probe.path).delete();
                probe.cacheFile.delete();
                probeDir.delete();
            }

            it("writes an entry on the first compile", function (done) {
                createProbe();
                loadInWorker(probe, function (result) {
                    expect(result.error).toBeUndefined();
                    expect(result.value).toBe(probe.token);
                    expectValidEntry(probe.cacheFile);
                    done();
                });
            });

            it("consumes a valid entry without rewriting it", function (done) {
                expect(probe.cacheFile.setLastModified(UNTOUCHED_MTIME)).toBe(true);
                loadInWorker(probe, function (result) {
                    expect(result.value).toBe(probe.token);
                    expect(Number(probe.cacheFile.lastModified())).toBe(UNTOUCHED_MTIME);
                    done();
                });
            });

            it("rejects an entry whose payload checksum fails and rewrites it", function (done) {
                var bytes = readBytes(probe.cacheFile);
                var offset = HEADER_SIZE + Math.floor((bytes.length - HEADER_SIZE) / 2);
                writeByte(probe.cacheFile, offset, (bytes[offset] & 255) ^ 0xff);
                expect(probe.cacheFile.setLastModified(UNTOUCHED_MTIME)).toBe(true);
                loadInWorker(probe, function (result) {
                    expect(result.value).toBe(probe.token);
                    expect(Number(probe.cacheFile.lastModified())).not.toBe(UNTOUCHED_MTIME);
                    expectValidEntry(probe.cacheFile);
                    done();
                });
            });

            it("rewrites an entry V8 rejects", function (done) {
                // Corrupt V8's own header (its magic number) but keep our
                // checksum valid, so the entry reaches V8 and V8 rejects it.
                var bytes = readBytes(probe.cacheFile);
                var original = bytes[HEADER_SIZE] & 255;
                bytes[HEADER_SIZE] = ((original ^ 0xff) << 24) >> 24;
                writeByte(probe.cacheFile, HEADER_SIZE, original ^ 0xff);
                writeUint32(probe.cacheFile, 20, payloadCrc(bytes));
                expect(probe.cacheFile.setLastModified(UNTOUCHED_MTIME)).toBe(true);
                loadInWorker(probe, function (result) {
                    expect(result.value).toBe(probe.token);
                    expect(Number(probe.cacheFile.lastModified())).not.toBe(UNTOUCHED_MTIME);
                    var rewritten = expectValidEntry(probe.cacheFile);
                    expect(rewritten[HEADER_SIZE] & 255).toBe(original);
                    done();
                });
            });

            it("tolerates a missing cache directory", function (done) {
                deleteTree(cacheDir);
                expect(cacheDir.exists()).toBe(false);
                loadInWorker(probe, function (result) {
                    expect(result.value).toBe(probe.token);
                    expectValidEntry(probe.cacheFile);
                    removeProbe();
                    done();
                });
            });
        });
    });
});

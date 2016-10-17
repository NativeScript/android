'use strict';

let args = process.argv;
let dependencies = JSON.parse(args[2]);
let platformsDir = args[3];

const path = require("path"),
    fs = require("fs");

if (dependencies) {
    let platformDir = path.join(platformsDir, "android");
    let buildDir = path.join(platformDir, "build-tools");
    let useV8File = path.join(buildDir, 'useV8');

    try {
        fs.unlinkSync(useV8File);
    } catch (e) {

    }

    let useV8Symbols = false;

    for (let dependencyName in dependencies) {
        let dependency = dependencies[dependencyName];
        
        let isPlugin = !!dependency.nativescript;
        if (isPlugin) {
            let consumesV8Symbols = !!dependency.nativescript.useV8symbols;
            if (consumesV8Symbols) {
                useV8Symbols = true;
                break;
            }
        }
    }

    if (useV8Symbols) {
        fs.writeFileSync(useV8File, "1");
    }
}
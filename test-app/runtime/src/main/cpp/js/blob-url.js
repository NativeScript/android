const {
    ArrayPrototypePush,
    FunctionPrototypeCall,
    Map,
    MapPrototypeDelete,
    MapPrototypeGet,
    MapPrototypeSet,
    ObjectDefineProperty,
} = primordials;

// The searchParams accessor below outlives init, so the constructor it reaches
// for is captured now rather than looked up on the global at call time.
const URLSearchParamsCtor = URLSearchParams;

const BLOB_STORE = new Map();
URL.createObjectURL = function (object, options = null) {
    try {
        // Blob/File come from the app layer, not the runtime, so these stay
        // live lookups; the catch below covers them not existing yet.
        if (object instanceof Blob || object instanceof File) {
            const id = java.util.UUID.randomUUID().toString();
            const ret = `blob:nativescript/${id}`;
            MapPrototypeSet(BLOB_STORE, ret, {
                blob: object,
                type: object?.type,
                ext: options?.ext,
            });
            return ret;
        }
    } catch (error) {
        return null;
    }
    return null;
};
URL.revokeObjectURL = function (url) {
    MapPrototypeDelete(BLOB_STORE, url);
};
const InternalAccessor = class {};
InternalAccessor.getData = function (url) {
    return MapPrototypeGet(BLOB_STORE, url);
};
URL.InternalAccessor = InternalAccessor;

// Pushes the params object's serialization onto its URL and records it as the
// query string the params object is in sync with.
function writeBack(params) {
    const url = params._url;
    url.search = params.toString();
    url._searchParamsSource = url.search;
}

// Brings `params` back in line with its URL's current query string by mutating
// it in place: url.searchParams is a same-object accessor, so the one params
// object handed out for a URL has to survive every reparse of that URL. Only
// the raw methods captured at construction are used here — the public ones are
// wrapped to write back into the URL, which would clobber the very string being
// applied.
function resync(params) {
    const url = params._url;
    const search = url.search;
    if (url._searchParamsSource === search) {
        return;
    }
    const names = [];
    FunctionPrototypeCall(params._forEach, params, function (value, name) {
        ArrayPrototypePush(names, name);
    });
    for (let i = 0; i < names.length; i++) {
        FunctionPrototypeCall(params._delete, params, names[i]);
    }
    const parsed = new URLSearchParamsCtor(search);
    FunctionPrototypeCall(params._forEach, parsed, function (value, name) {
        FunctionPrototypeCall(params._append, params, name, value);
    });
    url._searchParamsSource = search;
}

ObjectDefineProperty(URL.prototype, 'searchParams', {
    get() {
        if (this._searchParams == null) {
            const params = new URLSearchParamsCtor(this.search);
            ObjectDefineProperty(this, '_searchParams', {
                enumerable: false,
                writable: true,
                configurable: true,
                value: params,
            });
            ObjectDefineProperty(this, '_searchParamsSource', {
                enumerable: false,
                writable: true,
                configurable: true,
                value: this.search,
            });
            ObjectDefineProperty(params, '_url', {
                enumerable: false,
                writable: false,
                value: this,
            });
            params._forEach = params.forEach;
            params._append = params.append;
            params.append = function (name, value) {
                resync(this);
                this._append(name, value);
                writeBack(this);
            };
            params._delete = params.delete;
            params.delete = function (name) {
                resync(this);
                this._delete(name);
                writeBack(this);
            };
            params._set = params.set;
            params.set = function (name, value) {
                resync(this);
                this._set(name, value);
                writeBack(this);
            };
            params._sort = params.sort;
            params.sort = function () {
                resync(this);
                this._sort();
                writeBack(this);
            };
        } else {
            resync(this._searchParams);
        }
        return this._searchParams;
    },
});

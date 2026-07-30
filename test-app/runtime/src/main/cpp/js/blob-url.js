const {
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
ObjectDefineProperty(URL.prototype, 'searchParams', {
    get() {
        if (this._searchParams == null) {
            this._searchParams = new URLSearchParamsCtor(this.search);
            ObjectDefineProperty(this._searchParams, '_url', {
                enumerable: false,
                writable: false,
                value: this,
            });
            this._searchParams._append = this._searchParams.append;
            this._searchParams.append = function (name, value) {
                this._append(name, value);
                this._url.search = this.toString();
            };
            this._searchParams._delete = this._searchParams.delete;
            this._searchParams.delete = function (name) {
                this._delete(name);
                this._url.search = this.toString();
            };
            this._searchParams._set = this._searchParams.set;
            this._searchParams.set = function (name, value) {
                this._set(name, value);
                this._url.search = this.toString();
            };
            this._searchParams._sort = this._searchParams.sort;
            this._searchParams.sort = function () {
                this._sort();
                this._url.search = this.toString();
            };
        }
        return this._searchParams;
    },
});

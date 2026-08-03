const {
    ArrayIsArray,
    ArrayPrototypeForEach,
    Date,
    DatePrototypeToJSON,
    ObjectKeys,
} = primordials;

function serialize(data) {
    let store;
    switch (typeof data) {
        case "string":
        case "boolean":
        case "number": {
            return data;
        }
        case "object": {
            if (!data) {
                return null;
            }

            if (data instanceof Date) {
                return DatePrototypeToJSON(data);
            }

            if (ArrayIsArray(data)) {
                store = new org.json.JSONArray();
                ArrayPrototypeForEach(data, (item) => store.put(serialize(item)));
                return store;
            }

            store = new org.json.JSONObject();
            ArrayPrototypeForEach(ObjectKeys(data), (key) => store.put(key, serialize(data[key])));
            return store;
        }
        default:
            return null;
    }
}
module.exports = serialize;

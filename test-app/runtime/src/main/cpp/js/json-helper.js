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
                return data.toJSON();
            }

            if (Array.isArray(data)) {
                store = new org.json.JSONArray();
                data.forEach((item) => store.put(serialize(item)));
                return store;
            }

            store = new org.json.JSONObject();
            Object.keys(data).forEach((key) => store.put(key, serialize(data[key])));
            return store;
        }
        default:
            return null;
    }
}
module.exports = serialize;

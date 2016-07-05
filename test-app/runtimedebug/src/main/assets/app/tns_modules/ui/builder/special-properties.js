var specialProperties = new Map();
function specialPropertyKey(name) {
    return name.toLowerCase();
}
function registerSpecialProperty(name, setter) {
    var propertyKey = specialPropertyKey(name);
    if (specialProperties.has(propertyKey)) {
        throw new Error("Property for " + propertyKey + " already registered");
    }
    else {
        specialProperties.set(propertyKey, setter);
    }
}
exports.registerSpecialProperty = registerSpecialProperty;
function getSpecialPropertySetter(name) {
    var propertyKey = specialPropertyKey(name);
    return specialProperties.get(propertyKey);
}
exports.getSpecialPropertySetter = getSpecialPropertySetter;

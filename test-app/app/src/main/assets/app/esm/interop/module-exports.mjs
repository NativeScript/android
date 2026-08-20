// An arbitrary-string export name claims the whole require() result.
function theExport(a, b) {
    return a + b;
}

theExport.marker = "module.exports fixture";

export { theExport as "module.exports" };

export default "ignored when module.exports is present";

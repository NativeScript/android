try {
    require('./badJSON.json');
} catch (e) {
    TNSLog(e.message);
}

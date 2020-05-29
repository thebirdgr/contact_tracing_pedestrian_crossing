var mongoose = require("mongoose");

var tempidSchema = new mongoose.Schema({
    tempid: String
});

module.exports = mongoose.model("Tempid", tempidSchema);

var mongoose = require("mongoose");

// file to create the schema to follow in the database
var tempidSchema = new mongoose.Schema({
    tempid: String
});

// export the the schema
module.exports = mongoose.model("Tempid", tempidSchema);

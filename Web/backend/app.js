var express = require("express"),
    app = express(),
    bodyParser = require("body-parser"),
    mongoose = require("mongoose"),
    Tempid = require("./models/tempid");
var seedDB = require('./seed');

seedDB.seed();;
mongoose.connect("mongodb://localhost:27017/tempid", {useNewUrlParser: true});
app.set("view engine", "ejs");
app.use(express.static(__dirname + "/public"));
console.log(__dirname + "/public");

app.get('/', function(req,res){
    Tempid.find({}, (err, tempid) => {
        if(err){
            console.log(err);
        }else{
            // res.render("main", {tempid: tempid});
            res.json(tempid[tempid.length - 1].tempid);
        }
    })
});

app.post('/ap', function(req, res){
    let tempid = {
        tempid: seedDB.randomString(84)
    }
    Tempid.create(tempid, (err, tempid) => {
        if(err){
            console.log(err);
        }else{
            console.log("New Temporary ID created.");
            res.redirect('/');
        }
    })
})

app.listen(80, function(){
    console.log("Generator has started"); 
});
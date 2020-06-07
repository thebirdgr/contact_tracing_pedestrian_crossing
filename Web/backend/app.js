var express = require("express"),
    app = express(),
    bodyParser = require("body-parser"),
    mongoose = require("mongoose"),
    Tempid = require("./models/tempid");

// this allows the database seed functions to be included in this file    
var seedDB = require('./seed');

// calling the seed method
seedDB.seed();
// creating a database called tempid
mongoose.connect("mongodb://localhost:27017/tempid", {useNewUrlParser: true});
// to set the file to look for, i.e Embedded Javascript
app.set("view engine", "ejs");
// where to look for stylesheets and script files in public folder
app.use(express.static(__dirname + "/public"));

// the GET route
app.get('/', function(req,res){
    // a mongoose method
    Tempid.find({}, (err, tempid) => {
        if(err){
            console.log(err);
        }else{
            // this renders the main.ejs file, while passing tempid to the main file as "tempid"
            // res.render("main", {tempid: tempid}); 
            // this responds to the route as JSON and displays on site
            res.json(tempid[tempid.length - 1].tempid);
        }
    })
});

// the POST route
app.post('/ap', function(req, res){
    // generates a tempid object
    let tempid = {
        tempid: seedDB.randomString(84)
    }
    // mongoose method
    Tempid.create(tempid, (err, tempid) => {
        if(err){
            console.log(err);
        }else{
            console.log("New Temporary ID created.");
            // redirects to home page i.e. the GET route
            res.redirect('/');
        }
    })
})

// starts server on port 80
app.listen(80, function(){
    console.log("Generator has started"); 
});
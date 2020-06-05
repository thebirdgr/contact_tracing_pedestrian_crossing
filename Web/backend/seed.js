var mongoose = require("mongoose");

// include the schema
const Tempid = require('./models/tempid');

// declaring a function to generate a random alpha numeric string
// doesn't have quotations to prevent crashing
function randomString(length) {
    var result = '';
    var chars = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789~`!@#$%^&*()_+-={}[]:;\<>?,./|\\'
    for (var i = length; i > 0; --i) result += chars[Math.round(Math.random() * (chars.length - 1))];
    return result;
}


// const alphnum = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789~`!@#$%^&*()_+-={}[]:;\<>?,./|\\'

//Seed data 
data = [
    {
        tempid: randomString(84)
    }
]

// function that seeds the database with the data available
function seed() {
    data.forEach(tempid => {
        Tempid.create(tempid, (err, tempid) => {
            if (err) {
                console.log(err);
            } else {
                console.log("Generating Temporary ID");
                console.log(tempid);
                tempid.save();
                console.log("ID generated");
            }
        })
    })

}


// function seed(){
    //remove the temp id
//     Tempid.remove({}, (err) => {
//         if(err){
//             console.log(err);
//         } else{
//             Tempid.create(data[0], function(err, tempid){
//                 if(err){
//                     console.log(err);
//                 }else{
//                     console.log("Generating Temporary ID");
//                     console.log(tempid);
//                     tempid.save();
//                     console.log("ID generated");
//                 }
//             })
//         }
//     })
// }


// eporting both function seed and random string
module.exports = {
    seed,
    randomString
};
var mongoose = require("mongoose");

// include the schema
const Tempid = require('./models/tempid');

// declaring a function to generate a random alpha numeric string
// this function doesn't include the quotation characters to prevent crashing, it would interfere the entire string
function randomString(length) {
    var result = '';
    var chars = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789~`!@#$%^&*()_+-={}[]:;\<>?,./|\\'
    for (var i = length; i > 0; --i) result += chars[Math.round(Math.random() * (chars.length - 1))];
    return result;
}

// this is all the characters on the keyboard
// const alphnum = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789~`!@#$%^&*()_+-={}[]:;\<>?,./|\\'

//Seed data, data array with object
data = [
    {
        tempid: randomString(84)
    }
]

// function that seeds the databse with the data array available, gets updated every time a device is connected, does not restart
function seed() {
    data.forEach(tempid => {
        // .create() is a mongoose function 
        Tempid.create(tempid, (err, tempid) => {
            if (err) {
                console.log(err);
            } else {
                console.log("Generating Temporary ID");
                console.log(tempid);
                tempid.save(); // necessary to save the tempid that was created
                console.log("ID generated");
            }
        })
    })

}


// function seed(){
    //remove the temp ids
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


// exporting both the seed and randomString functions so that they can be used in other files
module.exports = {
    seed,
    randomString
};
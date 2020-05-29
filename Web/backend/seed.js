var mongoose = require("mongoose");
const Tempid = require('./models/tempid');

function randomString(length) {
    var result = '';
    var chars = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789~`!@#$%^&*()_+-={}[]:;\<>?,./|\\'
    for (var i = length; i > 0; --i) result += chars[Math.round(Math.random() * (chars.length - 1))];
    return result;
}

// doesn't have quotations to prevent crashing
// const alphnum = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789~`!@#$%^&*()_+-={}[]:;\<>?,./|\\'

//Seed data jHO^i%UA.~]wrZM_ap'l)&T64q&09P0@{Z53jqb2v3LxlPu5)!K@_f996p8NOjrZ;]guGoc!TrGyaJ00Hp'I
data = [
    {
        tempid: randomString(84)
    }
]


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
//     //remove the temp id
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

module.exports = {
    seed,
    randomString
};
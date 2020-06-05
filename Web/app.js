// this file was for testing purposes before being inlcuded in the database

function randomString(length, chars) {
    var result = '';
    for (var i = length; i > 0; --i) result += chars[Math.round(Math.random() * (chars.length - 1))];
    return result;
}
var tempid = document.getElementById('tempid');
//change the number to string
tempid.innerHTML = randomString(84, '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789~`!@#$%^&*()_+-={}[]:";\'<>?,./|\\');



// function randomString(length, chars) {
//     var mask = '';
//     if (chars.indexOf('a') > -1) mask += 'abcdefghijklmnopqrstuvwxyz';
//     if (chars.indexOf('A') > -1) mask += 'ABCDEFGHIJKLMNOPQRSTUVWXYZ';
//     if (chars.indexOf('#') > -1) mask += '0123456789';
//     if (chars.indexOf('!') > -1) mask += '~`!@#$%^&*()_+-={}[]:";\'<>?,./|\\';
//     var result = '';
//     for (var i = length; i > 0; --i) result += mask[Math.floor(Math.random() * mask.length)];
//     return result;
// }

// console.log(randomString(16, 'aA'));
// console.log(randomString(32, '#aA'));
// console.log(randomString(64, '#A!'));
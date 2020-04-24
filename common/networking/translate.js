function validateIP(ip) {
    var octets = [];

    for (var i = 0; i < 4; i++) {
        var octet = Math.floor(ip / Math.pow(256, 3 - i))
        ip -= octet * Math.pow(256, 3 - i)
        console.log(octet,ip)
        octets.push(octet)
    }

 	console.log(octets.join('.'))
}
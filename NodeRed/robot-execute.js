


module.exports = function(RED) {
    "use strict";

    function RobotExecuteNode(n) {
        RED.nodes.createNode(this,n);

        var node = this;

        // respond to inputs....
        this.on('input', function (msg) {

            var data = msg.payload.commands;

            var interval = 0;

            var msgs = [];
            for(var i in data) {

              // get command i
              node.warn("Data " + i + " " + JSON.stringify( data[i]) );
              msg.payload = data[i];

              msgs[i] = JSON.stringify(msg);

              // Shoot message with delay
              setTimeout( function(prm){ node.send( JSON.parse( prm ) ); }, interval, msgs[i] );

              // Add up next interval as setTimeout is not blocking
              interval = interval + parseInt(data[i].mdata[2]);
            }
        });

        this.on("close", function() {

        });
    }


    RED.nodes.registerType("robot-execute",RobotExecuteNode);

}

//
//  
//


module.exports = function(RED) {
    "use strict";
    
    // The node definition
    function RobotCommandNode(n) {
        RED.nodes.createNode(this,n);

        // Store node configuration properties
        this.time = n.time;
        this.command = n.command;

        var node = this;

        node.log("Time: " + this.time);
        node.log("Command: " + this.command);

        //var msg = {command: "drive", mdata:[26, 26, this.time]};
        //msg.topic = this.topic;
        //msg.payload = msg;


        // send out the message to the rest of the workspace.
        // ... this message will get sent at startup so you may not see it in a debug node.
        //this.send(msg);

        // respond to inputs....
        this.on('input', function (msg) {
            node.warn("command: " + this.command);

            var cmd;
            switch( this.command ) {
              case "Forward":
              case "fwd":
                cmd = {"command": "drive", "mdata" : [26, 26, this.time]};
              break;
              case "Left":
              case "left":
                cmd = {"command": "drive", "mdata" : [-26, 26, this.time]};
              break;
              case "Right":
              case "right":
                cmd = {"command": "drive", "mdata" : [26, -26, this.time]};
              break;
              case "Backward":
              case "bwd":
                cmd = {"command": "drive", "mdata" : [-26, -26, this.time]};
              break;
              default:
                node.error("Unknown command: " + this.command );
            }

            // Create empty array if payload is not defined
            // TODO: check if the payload is valid before adding next command
            //
            if (!msg.payload) {
                msg.payload = {'commands' : []}
            }
            msg.payload['commands'].push(cmd);

            node.warn("Payload: " + JSON.stringify( msg.payload ));
            node.send(msg);
        });

        this.on("close", function() {
            // Called when the node is shutdown - eg on redeploy.
            // Allows ports to be closed, connections dropped etc.
            // eg: node.client.disconnect();
        });
    }

    // Register the node by name. This must be called before overriding any of the
    // Node functions.
    RED.nodes.registerType("robot-command",RobotCommandNode);

}

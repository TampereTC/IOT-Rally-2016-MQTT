/**
 * Copyright 2014 IBM Corp.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 **/

// If you use this as a template, update the copyright with your own name.

// Sample Node-RED node file


module.exports = function(RED) {
    "use strict";
    // require any external libraries we may need....
    //var foo = require("foo-library");

    // The node definition
    function RobotCommandNode(n) {
        // Create a RED node
        RED.nodes.createNode(this,n);

        // Store node configuration properties
        this.time = n.time;
        this.command = n.command;

        // copy "this" object in case we need it in context of callbacks of other functions.
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

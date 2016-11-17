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
    function RobotExecuteNode(n) {
        // Create a RED node
        RED.nodes.createNode(this,n);

        // copy "this" object in case we need it in context of callbacks of other functions.
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
            // Called when the node is shutdown - eg on redeploy.
            // Allows ports to be closed, connections dropped etc.
            // eg: node.client.disconnect();
        });
    }

    // Register the node by name. This must be called before overriding any of the
    // Node functions.
    RED.nodes.registerType("robot-execute",RobotExecuteNode);

}

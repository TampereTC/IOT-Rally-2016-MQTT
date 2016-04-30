msgs = []

//Copy code.org code below

moveForward();
turnRight();
turnLeft();

//code.org code ends

sendAllMessages();

function moveForward() {
    msgs[msgs.length] = {payload: {command: "drive", mdata:[26, 26, 500]}};
}

function turnRight() {
    msgs[msgs.length] = {payload: {command: "drive", mdata:[-26, 26, 200]}};
}

function turnLeft() {
    msgs[msgs.length] = {payload: {command: "drive", mdata:[26, -26, 200]}};
}

var j = 0;
function sendSingleMessage() {
    node.send(msgs[j]);
    j++;
}

function sendAllMessages() {
    var interval = 1000;
    var i = 0;

    while(i < msgs.length) {
        setTimeout(sendSingleMessage, interval * i);
        i++;
    }

}

return;
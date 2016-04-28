msgs = []

//Copy code.org code below

moveForward();
turnRight();
turnLeft();

//code.org code ends

sendAllMessages();

function moveForward() {
    msgs[msgs.length] = {payload: {command: "drive", mdata:[100, 100, 500]}};
}

function turnRight() {
    msgs[msgs.length] = {payload: {command: "drive", mdata:[-100, 100, 500]}};
}

function turnLeft() {
    msgs[msgs.length] = {payload: {command: "drive", mdata:[100, -100, 500]}};
}

function sendSingleMessage() {
    node.send(msgs[global.get("i")]);
    global.set("i", global.get("i") + 1);
}

function sendAllMessages() {
    var interval = 2000;
    global.set("i", 0);
    var i = 0;

    while(i < msgs.length) {
        setTimeout(sendSingleMessage, interval * i);
        i++;
    }

}

return;
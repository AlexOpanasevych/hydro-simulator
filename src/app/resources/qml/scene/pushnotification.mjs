

WorkerScript.onMessage = function (message) {
    if(message.type === "create") {
//        log("create")
        message.model.append({'text' : message.text})

    }
    else {
//        log("remove")
        message.model.remove(0)

    }
}

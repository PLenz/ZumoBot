;window.zumo = (function(){
    var connection = new WebSocket('ws://' + location.hostname +':8082');

    function send(message) {
        //Check connection readyState
        if(connection.readyState !== 1){
            return;
        }

        var json = JSON.stringify(message);
            connection.send(json);
            console.log(message);
        }

        function move(x, xneg, y, yneg){
            send({
                    mode : 1,
                    data : [x, xneg, y, yneg]
            });
        }

        function mode(state){
            send({
                    mode : 0,
                    data : [state]
            });
        }

        function playSound(state) {
             send({
                    mode : 2,
                    data : [state]
            });
        }

        function driveMode(state) {
             send({
                    mode : 0,
                    data : [state]
            });
        }

        function lineMode(state) {
             send({
                    mode : 0,
                    data : [state]
            });
        }

        function autoPilot(state) {
             send({
                    mode : 0,
                    data : [state]
            });
        }

    return {
            move : move,
            playSound : playSound,
            driveMode : driveMode,
            lineMode  : lineMode,
            autoPilot : autoPilot            
    };

})();

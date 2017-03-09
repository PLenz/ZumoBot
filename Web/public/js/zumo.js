  /**
   * @see https://addyosmani.com/resources/essentialjsdesignpatterns/book/#revealingmodulepatternjavascript
   */
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

    return {
            move : move,
    };
})();
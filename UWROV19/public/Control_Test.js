window.addEventListener("gamepadconnected", function (e) {
            console.log("Gamepad connected, gamepad is: " +
            e.gamepad.id);
            $('.id').html(e.gamepad.id);
            $('.index').html(e.gamepad.index);
            $('.map').html(e.gamepad.mapping);
            $('.con').html(e.gamepad.connected);
            var buttons = e.gamepad.buttons;
            $('.buttons').html(buttons.length);
            $('.time').html(e.gamepad.timestamp);
        });
        window.addEventListener("gamepaddisconnected", function (e) {
            alert("You can't play without a gamepad");
        });
	const io = require('/usr/local/lib/node_modules/socket.io-client');
	const socket = io();
        var gamepad_count = 0;
        //var rest = 0.003921627998352051;
	var rest = -1.00; 
		var msg = ""; //Message to be sent over the USB (port call) to Teensy
        function pollGamepads() {
			var gamepads = navigator.getGamepads();
			msg = String((gamepads[0].axes[0] - rest).toFixed(3)) + String((gamepads[0].axes[2] - rest).toFixed(3)) + String((gamepads[0].axes[1] - rest).toFixed(3)) + "0.000" + "0.000" + String((gamepads[0].axes[3] - rest).toFixed(3)) + "?";
			//console.log(msg);
			socket.emit('send',msg);
		}
        setInterval(pollGamepads, 200);

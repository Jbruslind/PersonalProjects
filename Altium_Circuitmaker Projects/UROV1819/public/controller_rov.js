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

        var gamepad_count = 0;
        var rest = 0.003921627998352051;
        function pollGamepads() {
            var gamepads = navigator.getGamepads();
            //if (gamepads.length != gamepad_count) {
            gamepad_count = gamepads.length;
            //console.log("You have " + gamepad_count + " Gamepad Connected")
            //for (var i = 0; i < gamepads.length; i++) {
            //console.log("ID:" + gamepads[0].id);
            if (gamepads[0].buttons[0].pressed) {
                console.log("Button A pressed");
                //Socket.io calls here
            } else if (gamepads[0].buttons[1].pressed) {
                console.log("Button B pressed");
                //Socket.io calls here
            } else if (gamepads[0].buttons[2].pressed) {
                console.log("Button 2 pressed");
                //Socket.io calls here
            } else if (gamepads[0].buttons[3].pressed) {
                console.log("Button X pressed");
                //Socket.io calls here
            } else if (gamepads[0].buttons[4].pressed) {
                console.log("Button Y pressed");
                //Socket.io calls here
            } else if (gamepads[0].buttons[5].pressed) {
                console.log("Button 5 pressed");
                //Socket.io calls here
            } else if (gamepads[0].buttons[6].pressed) {
                console.log("Button Left Bumper pressed");
                //Socket.io calls here
            } else if (gamepads[0].buttons[7].pressed) {
                console.log("Button Right Bumper pressed");
                //Socket.io calls here
            } else if (gamepads[0].buttons[8].pressed) {
                console.log("Button Left Trigger pressed");
                //Socket.io calls here
            } else if (gamepads[0].buttons[9].pressed) {
                console.log("Button Right Trigger pressed");
                //Socket.io calls here
            } else if (gamepads[0].buttons[10].pressed) {
                console.log("Button Select pressed");
                //Socket.io calls here
            } else if (gamepads[0].buttons[11].pressed) {
                console.log("Button start pressed");
                //Socket.io calls here
            } else if (gamepads[0].buttons[12].pressed) {
                console.log("Button 12 pressed");
                //Socket.io calls here
            } else if (gamepads[0].buttons[13].pressed) {
                console.log("Button Left Thumbstick pressed");
                //Socket.io calls here
            } else if (gamepads[0].buttons[14].pressed) {
                console.log("Button Right Thumbstick pressed");
                //Socket.io calls here
            } else if (gamepads[0].buttons[15].pressed) {
                console.log("Button 15 pressed");
                //Socket.io calls here
            }

                   
            /*
              
              
                    0 /@@@@\ 1
                       @@@@
                    5 |@@@@| 2
                       @@@@
                    4 \@@@@/ 3
           **motor Thrust in 0 direction will be +** 
                        
                          Left Thumbstick Operation || gamepads[0].axes[0] && gamepads[0].axes[1]
            Left: Left Strafe----motor0: -, motor3: -, motor1: +, motor4: -
            Right: Right Strafe----motor0: +, motor3 = +, motor1: -, motor4 ++
            Up: Forward Thrust----motor0: -, motor1 = -, motor3: -, motor4: -
            Down: Reverse Thrust----motor0: +, motor1: +, motor4: +, motor3: +
            */        
            
            if (gamepads[0].axes[0] != rest)
            {
                console.log("Left Thumbstick Left/Right: ", gamepads[0].axes[0]); //Left = -1 Right = 1 Mid = 0.0039...
                if (gamepads[0].axes[0] < rest)
                {
                    console.log("Left Strafe at ", gamepads[0].axes[0], "power");                            
                } else if (gamepads[0].axes[0] > rest)
                {
                    console.log("Right Strafe at ", gamepads[0].axes[0], "power");
                }
            }

            if (gamepads[0].axes[1] != rest)
            {
                console.log("Left Thumbstick Up/Down: ", gamepads[0].axes[0]); //Up = -1 Down = 1 Mid = 0.0039...
                if (gamepads[0].axes[1] < rest)
                {
                    console.log("Forward Thrust at ", gamepads[0].axes[1], "power");
                } else if (gamepads[0].axes[1] > rest)
                {
                    console.log("Reverse Thrust at ", gamepads[0].axes[1], " power");
                }
            }



            /*
                    0 /@@@@\ 1
                       @@@@
                    5 |@@@@| 2
                       @@@@
                    4 \@@@@/ 3
              **motor Thrust in 0 direction will be +**
            **motor5 and motor2 downward direction per  bot will be -**
                        
                    Right Thumbstick || gamepads[0].axes[2] && gamepads[0].axes[5]
            Left: Left Turn :: motor0: +, motor3: -,   
            Right: Right Turn :: motor1: +, motor4: - 
            Up: Upward Thrust :: motor5: -, motor2: -
            Down: Downward Thrust :: motor5: +, motor2: +
            */
            if (gamepads[0].axes[2] != rest)
            {
                console.log("Right Thumbstick Left/Right: ", gamepads[0].axes[2]); //Left = -1 Right = 1 Mid = 0.0039...                        
                if (gamepads[0].axes[2] < rest)
                {
                    console.log("Left turn at ", gamepads[0].axes[2], " power");
                } else if(gamepads[0].axes[2] > rest)
                {
                    console.log("Right turn at ", gamepads[0].axes[2], " power");
                }
            }

            if(gamepads[0].axes[5] != rest)
            {
                if(gamepads[0].axes[5] < rest)
                {
                    console.log("Thurst Up at ", gamepads[0].axes[5], " power");
                } else if(gamepads[0].axes[5] > rest)
                {
                    console.log("Thrust Down at ", gamepads[0].axes[5], " power");
                }
            }

                             
            /*
                             0 /@@@@\ 1
                                @@@@
                             5 |@@@@| 2
                                @@@@
                             4 \@@@@/ 3
     
     
                    **motor thrust in 0 direction will be +**
                    **motor5 and motor2 downward direction per  bot will be -**
            Right Trigger: Bank Right: motor5: -, motor2: +
            Left Trigger: Bank Left: motor5: +, motor2 -
            */

               console.log("Right Trigger Sens: ", gamepads[0].axes[3]); //button not pressed = -1 button fully pressed = 1
               console.log("Left Trigger Sens: ", gamepads[0].axes[4]); //-1 = button not pressed 1 = button fully pressed


            if (gamepads[0].axes[3] != -1)
            {
                console.log("Bank right at ", gamepads[0].axes[3], " power");
            }
                    
            if(gamepads[0].axes[4] != -1)
            {
                console.log("Bank Left at ", gamepads[0].axes[4], " power");
            }

            /*
                  The manipulator is a scara type arm.
          Movement will be incremental with option for propertyIsEnumerable
              All operation will be with the D-Pad

@@@@@@@@@@                          +
 @@@@@@@@                          + ++
 @@@@@@@@+++++++++++++++++++++^^^^+ +  +
 @@@@@@@@+++++++++++++++++++++^^^^ +    
 @@@@@@@@+++++++++++++++++++++<><>
 @@@@@@@@+++++++++++++++++++++<><>+     +
                                   ++ ++ 
                                     +
   
   This should be the arm at rest I would perfer that this is a const int ARM_ANGLE_ZERO  
   

              D-Up: +10 degrees
              D-Down: -10 degrees
              D-Left: Spin Left +10 degrees
              D-RightL Spin Left +10 degrees
          
          */

     //console.log("D-Pad: ", gamepads[0].axes[9]); // Up = -1, Down = 0.142... Left = 0.714... Right = -0.428 
           
            var d_up = -1;
            var d_left = 0.7142857313156128;
            var d_right = -0.4285714030265808;
            var d_down = 0.14285719394683838;
            var d_rest = -1.2857142686843872;

            if (gamepads[0].axes[9] != d_rest)
                {
            if (gamepads[0].axes[9] === d_up)
            {
                console.log("Servo arm up");
            } else if (gamepads[0].axes[9] === d_down) 
            {
                console.log("Servo arm down");
            } else if (gamepads[0].axes[9] === d_left) 
            {
                console.log("Servo arm Spin Left");
            } else if (gamepads[0].axes[9] === d_right) 
            {
                console.log("Servo arm Spin Right");
            }
            }
            }
        setInterval(pollGamepads, 200);

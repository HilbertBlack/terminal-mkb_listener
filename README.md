# terminal-mkb_listener
This library holds the necessory classes and funtions for capturing and
handling the teminal events like mouse and keyboard inputs.
------------------------------------------------------------------------

Anyone just need to copy this "mkblistener" file into their directory.
need to add this file to their working code as { #include "mklistener" }.

Once done that create a MAIN class or any main working classs 
and inherit the "KEYBOARD_MOUSE_LISTENER" class which is present in 
the "mkblistener" file.


    class MAIN : public KEYBOARD_MOUSE_LISTENER
    {
    
      void mousePressed(MOUSE_EVENT e) override
      
      {
        // override the functions by calling them...
      }
      
      void keyPressedd(unsigned char key) override
      
      {
        // override to use it...
      }
    
    }

THAT'S DONE.. you can start the operations by calling the 
"listen" function. 

    class MAIN mkb;
    
    mkb.listen();

----------------------------------------------------------

FEATURES THIS CLASS CAN HANDLE

* mouse button press ( left, middle, right )
* mouse move
* mouse drag ( left, middle, right )
* wheel move ( up , down)
* Keyboard key hit.

NOTE: For multi-bytes key inputs like the arrows and function keys,
      the "keyPressed( key )" function is called for each of the multiple
      bytes. To identify those unique keys you have to loop through and 
      have to use flag variables.


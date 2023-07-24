// receiver.pde

#include <VirtualWire.h>

const int receive_pin = 11;




void setup()

{

    delay(1000);

    Serial.begin(9600);  // Debugging only

    Serial.println("setup");




    // Initialise the IO and ISR

    vw_set_rx_pin(receive_pin);

    vw_set_ptt_inverted(true); // Required for DR3100

    vw_setup(2000);     // Bits per sec

    vw_rx_start();       // Start the receiver PLL running




}




void loop()

{

    uint8_t buf[VW_MAX_MESSAGE_LEN];

    uint8_t buflen = VW_MAX_MESSAGE_LEN;




    if (vw_get_message(buf, &buflen)) // Non-blocking

    {

            int i;

            // Message with a good checksum received, dump it.

            //Serial.print("Got: ");

           

            for (i = 0; i < buflen; i++)

            {

                char c = (buf[i]);

    if( c == 'a')

    {




                Serial.println("1001");

delay(1000);

          

    }

        if( c == 'b') // for right side

    {




                Serial.println("1002");

delay(1000);

          

    }




            }




    }

}

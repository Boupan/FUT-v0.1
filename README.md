# FUT-v0.1

_Found You Thief_ is a novice anti-theft system which informs you about the whereabouts of your vehicle upon request.

It features a **SIM800L module**, a **NEO 6M GPS module** and an **Arduino Pro mini** as its MCU.

### Apparatus Logic

Once the user wants to know the location, he sends a SMS or he simply calls the number of the SIM. Then the system wakes up the Pro mini and starts up the GPS module. As soon as the location 
data are fetched, arduino turns them into a more readable format in the form of a **Google Maps** link and parses them over to the SIM module which then sends the SMS with the info needed. Then the Sim800L
goes to sleep followed by the arduino.

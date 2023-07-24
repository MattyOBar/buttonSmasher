#Use this to populate redis with data for debugging purposes

curl -X POST http://localhost:5000/add/game -F gameId=0 -F duration=15 -F score=6 -F name=MattyBoi
curl -X POST http://localhost:5000/add/game -F gameId=1 -F duration=15 -F score=69 -F name=Matty  
curl -X POST http://localhost:5000/add/game -F gameId=2 -F duration=30 -F score=32 -F name=TRASH  
curl -X POST http://localhost:5000/add/game -F gameId=3 -F duration=15 -F score=6 -F name=Mbutts 
curl -X POST http://localhost:5000/add/game -F gameId=4 -F duration=30 -F score=6 -F name=Mbutt6 
curl -X POST http://localhost:5000/add/game -F gameId=5 -F duration=15 -F score=17 -F name=Olivia
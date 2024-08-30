const express = require('express') ;
const path = require('path') ;
const fetch = require('node-fetch') ;

const app = express();
const port = 5000;

const organization_id = "" ;
const API_KEY = "" ;

headers = {
    "Authorization" : `Bearer ${API_KEY}`,
    "Accept" : "application/json",
} 
body = null ;

const get_URL = (serial_number) => `https://api.meraki.com/api/v1/organizations/${organization_id}/sensor/readings/latest?metrics[]=temperature&serials[]=${serial_number}` ;

const West_RDC = "" ;
const West_1 = "" ;

const East_RDC = "" ;
const East_1 = "" ;

const sensors = [
    {name : "West_RDC", sn : West_RDC}, 
    {name : "West_1", sn : West_1}, 
    {name : "East_RDC", sn : East_RDC}, 
    {name : "East_1", sn : East_1}] ;

app.use(express.static(path.join(__dirname, 'public'))) ;

app.get('/get_temp', async (req, res) => {
    let tab = {} ;

    for (const sensor of sensors) {
        try {
            const response = await fetch(get_URL(sensor.sn), {
                method : "GET",
                headers : headers,
                body : body 
            })
    
            const data = await response.json() ;
    
            tab[sensor.name] = data[0].readings[0].temperature.celsius ;
        }
        catch (error) {
            console.error("Erreur : ", error) ;
        }
    }
    res.send(JSON.stringify(tab)) ;
}) ;

app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, 'public', 'index_bloc.html')) ;
}) ;

app.listen(port, () => {
    console.log(`Server is running at http://localhost:${port}`) ;
}) ;
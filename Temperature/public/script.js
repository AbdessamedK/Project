async function get_temp() {
    try {
        const response = await fetch('/get_temp') ;
        const data = await response.json() ;
        
        const sensors = Object.keys(data) ;

        for (const sensor of sensors) {
            let temp = data[sensor].toFixed(1).replace('.', ',') ;
            const $div = document.querySelector(`#${sensor}`) ;
            $div.innerHTML = `<span>Température:</span> ${temp}°C` ;
        }
    }
    catch(error) {
        console.error("Erreur : ", error) ;
    }
}

setInterval(get_temp, 2000) ;
const $formulaire = document.querySelector("form") ;
const $table = document.querySelector("#tableau_lien tbody") ;
const $select = document.querySelector("#group") ;

async function cree_lien() {
    $formulaire.addEventListener("submit", async function(event) {
        event.preventDefault() ;
        
        const group = $select.value ;
        const max_accounts = $formulaire.querySelector("#max-accounts").value ;

        try {
            const response = await fetch(generate_link_url, {
                method : "POST",
                headers : {
                    "Content-Type" : "application/json"
                },
                body : JSON.stringify({group : group, max_accounts : max_accounts})     
            }) ;

            const data = await response.json() ;

            if (data.success) {
                alert("Lien généré avec succès !") ;
            }

            location.reload() ;
        }
        catch(error) {
            console.error("Erreur : ", error) ;
        }
    })
}

async function getListSharedAccount() {
    try {
        const response = await fetch(get_list_url, {
            method : "POST", 
            headers : {
                "Content-Type" : "application/json"
            }
        }) ;
        
        const data = await response.json() ;

        for (const group of data.liste) {
            const $option = document.createElement("option") ;
            $option.value = group ;
            $option.textContent = group ;

            $select.append($option) ;
        }
    }
    catch(error) {
        console.error("Erreur : ", error) ;
    }
}

async function loadLinks() {
    try {
        const response = await fetch(get_all_url, {
            method : "POST",
            headers : {
                "Content-Type" : "application/json"
            }
        }) ;

        const data = await response.json() ;

        for (const tuple of data.liste) {
            const $tr = document.createElement("tr") ;
            let $td = document.createElement("td") ;
            const $a = document.createElement("a") ;
            
            const $bouton = document.createElement("button") ;
            $bouton.classList.add("supp") ;
            $bouton.textContent = "✖" ;
            $bouton.lien = tuple[0] ;

            $bouton.addEventListener("click", async function() {
                try {
                    const response = await fetch(delete_url, {
                        method : "POST",
                        headers : {
                            "Content-Type" : "application/json"
                        },
                        body : JSON.stringify({
                            link : $bouton.lien
                        })
                    }) ;

                    const data = await response.json() ;

                    if (data.success) {
                        location.reload() ;
                    }
                    else {
                        alert("Erreur lors de la suppression du lien !") ;
                    }
                }
                catch(error) {
                    console.error("Erreur : ", error) ;
                    alert("Erreur lors de la suppression du lien !") ;
                }
            }) ;
            
            $a.href = tuple[0] ; $a.textContent = tuple[0] ;
            $td.append($a) ;
            $tr.append($td) ;

            for (let i=1; i<4; i++) {
                $td = document.createElement("td") ;
                $td.textContent = tuple[i] ;
                $tr.append($td) ;
            }

            $td = document.createElement("td") ;
            $td.append($bouton) ;
            $tr.append($td) ;

            $table.append($tr) ;
        }
    }
    catch(error) {
        console.error("Erreur : ", error) ;
    }
}

loadLinks() ;
getListSharedAccount() ;
cree_lien() ;
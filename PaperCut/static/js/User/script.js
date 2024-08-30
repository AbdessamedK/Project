const $formulaire = document.querySelector("#signupForm") ;
const $err_mdp = document.querySelector("#error") ;
const $err_nomc = document.querySelector("#error_nomc") ;
const $err_username = document.querySelector("#error_username") ;
const $err_email = document.querySelector("#error_email") ;
const $cond1 = document.querySelector("#cond1") ;
const $cond2 = document.querySelector("#cond2") ;

function clean_data(data) {
    return DOMPurify.sanitize(data) ;
}

async function submit() {
    $formulaire.addEventListener("submit", async function(event) {
        event.preventDefault() ;

        const $nomc = clean_data($formulaire.querySelector("#name").value.trim()) ;
        const $username = clean_data($formulaire.querySelector("#username").value.trim()) ;
        const $email = clean_data($formulaire.querySelector("#email").value.trim()) ;
        const $password = clean_data($formulaire.querySelector("#password").value.trim()) ;
        const $confirmpassword = clean_data($formulaire.querySelector("#confirmpassword").value.trim()) ;
        
        let pattern = /^[a-zA-ZÀ-ÖØ-öø-ÿ\-._ ]+$/; 
        if (!pattern.test($nomc)) {
            $err_nomc.style.display = 'block' ;
            return ;
        } else {
            $err_nomc.style.display = 'none' ;
        }

        if (!pattern.test($username)) {
            $err_username.style.display = 'block' ;
            return ;
        } else {
            $err_username.style.display = 'none' ;
        }

        if (!validator.isEmail($email)) {
            $err_email.style.display = 'block' ;
            return ;
        } else {
            $err_email.style.display = 'none' ;
        }
        
        pattern = /^(?!.*['"<>]).+$/;
        const regex2 = /[?,.;:!\-_#&%@(){}\[\]$]/;
        if (!pattern.test($password) || $password !== $confirmpassword || ($password.length < 8) || !regex2.test($password)) {
            $err_mdp.style.display = 'block' ;
            return ;
        } else {
            $err_mdp.style.display = 'none' ;
        }

        try {
            const response = await fetch(submit_url, {
                method : "POST",
                headers : {
                    "Content-Type" : "application/json"
                },
                body : JSON.stringify({
                    nomc : $nomc,
                    username : $username,
                    email : $email,
                    password : $password,
                    link : window.location.href 
                })
            }) ;

            const data = await response.json() ;

            if (data.success) {
                alert("Votre compte a bien été crée !") ;
            } else {
                alert("Erreur lors de la création du compte") ;
            }
            
            location.reload() ;
        }
        catch(error) {
            alert("Erreur serveur, veuillez réessayer plus tard") ;
            console.error("Erreur : ", error) ;
        }
    }) ;
}

function style() {
    $cond1.classList.add("invalide") ;
    $cond2.classList.add("invalide") ;

    const $password = document.querySelector("#password") ;
    const $confirmpassword = document.querySelector("#confirmpassword") ;
    let cond1 = false ;
    let cond2 = false ;

    $password.classList.add("input-rouge") ;
    $confirmpassword.classList.add("input-rouge") ;

    const regex2 = /[?,.;:!\-_#&%@(){}\[\]$]/; 
    const pattern = /^(?!.*['"<>]).+$/; // true si contient pas de chose bzr
    const vide = /^$/ ; // true si vide

    $password.addEventListener("input", function() {
        const value = clean_data($password.value.trim()) ;
        
        if (pattern.test(value)  || vide.test(value)) {

            if (!(value.length>=8)) {
                $cond1.classList.remove("valide") ;
                $cond1.classList.add("invalide") ;
                cond1 = false ;
            }
            else {
                $cond1.classList.remove("invalide") ;
                $cond1.classList.add("valide") ;
                cond1 = true ;
            }

            if (regex2.test(value)) {
                $cond2.classList.remove("invalide") ;
                $cond2.classList.add("valide") ;
                cond2 = true ;
            }
            else {
                $cond2.classList.remove("valide") ;
                $cond2.classList.add("invalide") ;
                cond2 = false ;
            }
        }

        if (cond1 && cond2) {
            $password.classList.remove("input-rouge") ;
            $password.classList.add("input-vert") ;
        }
        else {
            $password.classList.remove("input-vert") ;
            $password.classList.add("input-rouge") ;
        }
    }) ;

    $confirmpassword.addEventListener("input", function() {
        const confirmpasswd = clean_data($confirmpassword.value.trim()) ;
        const passwd = clean_data($password.value.trim()) ;

        if ((pattern.test(confirmpasswd)|| vide.test(confirmpasswd)) && pattern.test(passwd)) {
            if (passwd === confirmpasswd) {
                $confirmpassword.classList.remove("input-rouge") ;
                $confirmpassword.classList.add("input-vert") ;
            }
            else {
                $confirmpassword.classList.remove("input-vert") ;
                $confirmpassword.classList.add("input-rouge") ;
            }
        }
    }) ;
}

style() ;
submit() ;
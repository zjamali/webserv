
const hambergerIcon = document.querySelector("#hamburger-nav");
const xIcon = document.querySelector("#x-icon");
const navBar = document.querySelector("#ham-navbar");

hambergerIcon.addEventListener('click', function(e){

    hambergerIcon.classList.add("hide");
    hambergerIcon.classList.remove("display");
    xIcon.classList.remove("hide");
    xIcon.classList.add("display");
    navBar.classList.add("display");
    navBar.classList.remove("hide");
});

const links = document.querySelectorAll(".ham-links");

console.dir(links);

for (let link of links)
{
    link.addEventListener('click',function(e) {
        xIcon.classList.add("hide");
        xIcon.classList.remove("display");
        hambergerIcon.classList.remove("hide");
        hambergerIcon.classList.add("display");
        navBar.classList.add("hide");
        navBar.classList.remove("display");
    });
}

xIcon.addEventListener('click',function(e) {
    xIcon.classList.add("hide");
    xIcon.classList.remove("display");
    hambergerIcon.classList.remove("hide");
    hambergerIcon.classList.add("display");
    navBar.classList.add("hide");
    navBar.classList.remove("display");
})
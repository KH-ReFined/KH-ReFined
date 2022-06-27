function changePage()
{
    const md = new Remarkable({
        html:true
    });

    _fetchHash = window.location.hash;

    if (_fetchHash === "")
    {
        window.location.hash = "Main_Page";
    }

    document.title = "Kingdom Hearts - Re:Fixed | " + _fetchHash.substring(1).replace("_", " ");

    _currActive = document.querySelectorAll(".linkActive");
    _inactive = document.querySelectorAll("a[href='" + _fetchHash + "']")[0];

    for(var i = 0; i < _currActive.length; i++)
    {
        _currActive[i].classList.remove("linkActive");
    }

    _inactive.classList.add("linkActive");
    _inactive.parentNode.children[0].className += " linkActive";

    fetch('https://raw.githubusercontent.com/TopazTK/KH-ReFixed/website/md/index.md')
    .then(response=> response.text())
    .then(text=> document.getElementById('trueContent').innerHTML = md.render(text))
}
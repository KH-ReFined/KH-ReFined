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

    _firstContent = document.getElementById('firstContent');
    _secondContent = document.getElementById('secondContent');

    if (window.getComputedStyle(_firstContent).getPropertyValue("opacity") != 0)
    {
        _secondContent.scrollTop = 0;

        _firstContent.classList.add("inactive");    
        _secondContent.classList.remove("inactive");   

        _firstContent.style.pointerEvents = "none";
        _secondContent.style.pointerEvents = "all";

        _secondContent.focus();

        fetch('https://raw.githubusercontent.com/TopazTK/KH-ReFixed/website/md/' + _fetchHash.substring(1).toLowerCase().replace("_", "") +'.md')
        .then(response=> response.text())
        .then(text=> _secondContent.innerHTML = md.render(text))
    }

    else if (window.getComputedStyle(_secondContent).getPropertyValue("opacity") != 0)
    {
        _firstContent.scrollTop = 0;

        _firstContent.classList.remove("inactive");    
        _secondContent.classList.add("inactive"); 

        _firstContent.style.pointerEvents = "all";
        _secondContent.style.pointerEvents = "none";

        _firstContent.focus();

        fetch('https://raw.githubusercontent.com/TopazTK/KH-ReFixed/website/md/' + _fetchHash.substring(1).toLowerCase().replace("_", "") +'.md')
        .then(response=> response.text())
        .then(text=> _firstContent.innerHTML = md.render(text))
    }
}
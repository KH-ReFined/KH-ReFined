function changePage()
{
    const md = new Remarkable({
        html:true
    });

    _fetchHash = window.location.hash;

    document.title = "Kingdom Hearts - Re:Fixed | " + _fetchHash.substring(1).replace("_", " ");

    fetch('https://raw.githubusercontent.com/TopazTK/KH-ReFixed/website/md/index.md')
    .then(response=> response.text())
    .then(text=> document.getElementById('trueContent').innerHTML = md.render(text))
}
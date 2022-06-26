function changePage()
{
    _fetchHash = window.location.hash;


    document.title = "Kingdom Hearts - Re:Fixed | " + _fetchHash.substring(1).replace("_", " ");

    fetch('https://raw.githubusercontent.com/TopazTK/KH-ReFixed/website/index.html')
    .then(response=> response.text())
    .then(text=> document.getElementById('trueContent').innerHTML = text)
}
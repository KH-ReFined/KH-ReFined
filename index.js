function changePage()
{
    _fetchHash = window.location.hash;


    document.title = "Kingdom Hearts - Re:Fixed | " + _fetchHash.substring(1).replace("_", " ");

    fetch('index.html')
    .then(response=> response.text())
    .then(text=> document.getElementById('trueContent').innerHTML = text);
}
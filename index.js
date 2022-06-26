function changePage()
{
    _fetchHash = window.location.hash;


    document.title = "Kingdom Hearts - Re:Fixed | " + _fetchHash.substring(1).replace("_", " ");

    const url1 = 'https://raw.githubusercontent.com/TopazTK/KH-ReFixed/website/index.html'
    const response = fetch(url1);
    document.getElementById('trueContent').innerHTML= response.text;
}
function changePage()
{
    _fetchHash = window.location.hash;


    document.title = "Kingdom Hearts - Re:Fixed | " + _fetchHash.substring(1).replace("_", " ");

    $('#trueContent').load('index.html');
}
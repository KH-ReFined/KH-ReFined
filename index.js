function changePage()
{
    _fetchHash = window.location.hash;


    document.title = "Kingdom Hearts - Re:Fixed | " + _fetchHash.substring(1).replace("_", " ");

    var xhr= new XMLHttpRequest();
    xhr.withCredentials = true;
    xhr.open('GET', 'https://raw.githubusercontent.com/TopazTK/KH-ReFixed/website/index.html', true);
    xhr.setRequestHeader('X-Requested-With', 'XMLHttpRequest'); 
    xhr.setRequestHeader('Access-Control-Allow-Origin', '*');
    xhr.onreadystatechange= function() {
        if (this.readyState!==4) return;
        if (this.status!==200) return; // or whatever error handling you want
        document.getElementById('trueContent').innerHTML= this.responseText;
    };
    xhr.send();
}
const express   = require('express')
const fs        = require('fs')
const request   = require('request')
const cheerio   = require('cheerio')
const app       = express()

app.get('/', (req, res) => {
    res.send("https://caraguamoviesapi.herokuapp.com/api/v1")
})

app.get('/api/v1', (req, res) => {
    let url = 'http://www.adorocinema.com/filmes/numero-cinemas/'; 
    

    request(url, (error, response, html) => {
        if (!error) {
            
            let $ = cheerio.load(html)
            let result = []; 

            $('.mdl').each((i, element) => {
                let movie       = $(element)
                let title       = movie.find('.meta').find('.meta-title').find('a').text()
                let date        = movie.find('.meta').find('.meta-body').find('.meta-body-item').find('.date').text()
                let thumb       = movie.find('.card').find('.thumbnail').find('.thumbnail-container').find('img').data('src')
                let synopsis    = movie.find('synopsis').find('.content-txt').text()
                console.log(thumb)
                result.push({
                    'title'     : title,
                    'date'      : date,
                    'thumb'     : thumb, 
                    'synopsis'  : synopsis

                })
            })

    
            res.header("Content-Type",'application/json');
            res.send(JSON.stringify(result, null, 4));
        }
    })
})

app.get('/api/v2', (req, res) => {
    let url = 'http://www.centerplex.com.br/cinema/647/serramar-shopping.html'
    let sinopse = "http://www.centerplex.com.br/"

    request(url, (error, response, html) => {
        if (!error) {

            let $ = cheerio.load(html)

            let result = []
            $('.filme').each((i, element) => {
               var movieElement = $(element)
               var title = movieElement.find('h3').text()
               var time  = movieElement.find('ol').children('li').eq(1).text()
               var theme = movieElement.find('ol').children('li').eq(2).text()
               var sinopseLink = movieElement.find('ol').children('li').eq(4).find('a').attr('href')
            
               result.push({
                   'title' : title,
                   'time' : time, 
                   'theme' : theme
               })

            })

            res.send(result)

        }
    })
})

const PORT = "8081"
app.listen(PORT)

console.log('Server happens on port ' + PORT)



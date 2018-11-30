const express   = require('express')
const fs        = require('fs')
const request   = require('request')
const cheerio   = require('cheerio')
const app       = express()

app.get('/', (req, res) => {
    res.send("https://caraguamoviesapi.herokuapp.com/api/v1")
})

app.get('/api/v1', (req, res) => {
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

app.listen('8081')

console.log('Server happens on port 8081')


exports = module.exports = app;
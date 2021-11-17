/**
 * HTTP app.
 *
 * Instantiates and runs the Express app, which handles all HTTP endpoints.
 */

import cors from 'cors'
import express from 'express'
import * as path from 'path'

import apiScore from './routes/score'

// —————————————————
// Instantiating
// —————————————————

const app = express()
const PORT = process.env.PORT || 8888
app.set('port', PORT)

// —————————————————
// Middleware
// —————————————————

app.use(
    cors({
        credentials: true, // Allows credentials (cookies) to be set across origins
        origin: 'http://localhost:3000', // Allow CORS from local React server
    }),
)
app.use(express.text()) // Parse incoming requests as plaintext.

// —————————————————
// Serving
// —————————————————

// Serve the static files from the React app.
app.use(express.static(path.join(__dirname, '..', '..', 'frontend/build')))

// Handles any requests that don't match /api or /static.
app.get(/^\/(?!(api|static)).*/, (req, res) => {
    console.log('°°°°°°°°°')
    res.sendFile(path.join(__dirname, '..', '..', '/frontend/build/index.html'))
})

// —————————————————
// API Routes
// —————————————————

// Endpoints related to keeping score.
app.use('/api/score', apiScore)

// —————————————————
// Final Middleware
// —————————————————

// Handle errors
app.use((err: any, req: any, res: any, next: any) => {
    // set locals, only providing error in development
    res.locals.message = err.message
    res.locals.error = req.app.get('env') === 'development' ? err : {}

    res.status(err.status || res.statusCode || 500)
    // res.render('error');  // render the error page
    // res.json({
    //   message: err.message,
    //   error: req.app.get('env') === 'development' ? err : {}
    // })
    res.json({
        errors: [
            {
                msg: err.message,
                error: process.env.NODE_ENV === 'development' ? err : {},
            },
        ],
    })
})

// —————————————————
// Create Server
// —————————————————

import httpModule from 'http'

/**
 * Event handler for server 'error' event.
 */
 function onError(err: any) {
    if (err.syscall !== 'listen') {
        throw err
    }

    const bind = typeof PORT === 'string' ? 'pipe ' + PORT : 'port ' + PORT

    // handle specific listen errors with friendly messages
    switch (err.code) {
        case 'EACCES':
            console.error(bind + ' requires elevated privileges')
            process.exit(1)
            break
        case 'EADDRINUSE':
            console.error(bind + ' is already in use')
            process.exit(1)
            break
        default:
            throw new Error('oh dear')
    }
}

/**
 * Event listener for server listening event.
 */
const onListening = (server: any) => () => {
    const addr = server.address()
    const bind = typeof addr === 'string' ? 'pipe ' + addr : 'port ' + addr.port
    console.log('HTTP: Listening on', bind)
}

/**
 * Builds the server and starts the app.
 */
function start() {
    // Build a server; inject Express app.
    const server = new httpModule.Server(app)
    server.listen(PORT)
    server.on('error', onError)
    server.on('listening', onListening(server))
}

// —————————————————
// Start
// —————————————————

start()

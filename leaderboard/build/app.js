"use strict";
/**
 * HTTP app.
 *
 * Instantiates and runs the Express app, which handles all HTTP endpoints.
 */
var __createBinding = (this && this.__createBinding) || (Object.create ? (function(o, m, k, k2) {
    if (k2 === undefined) k2 = k;
    Object.defineProperty(o, k2, { enumerable: true, get: function() { return m[k]; } });
}) : (function(o, m, k, k2) {
    if (k2 === undefined) k2 = k;
    o[k2] = m[k];
}));
var __setModuleDefault = (this && this.__setModuleDefault) || (Object.create ? (function(o, v) {
    Object.defineProperty(o, "default", { enumerable: true, value: v });
}) : function(o, v) {
    o["default"] = v;
});
var __importStar = (this && this.__importStar) || function (mod) {
    if (mod && mod.__esModule) return mod;
    var result = {};
    if (mod != null) for (var k in mod) if (k !== "default" && Object.prototype.hasOwnProperty.call(mod, k)) __createBinding(result, mod, k);
    __setModuleDefault(result, mod);
    return result;
};
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
const cors_1 = __importDefault(require("cors"));
const express_1 = __importDefault(require("express"));
const path = __importStar(require("path"));
const score_1 = __importDefault(require("./routes/score"));
// —————————————————
// Instantiating
// —————————————————
const app = (0, express_1.default)();
const PORT = process.env.PORT || 8888;
app.set('port', PORT);
// —————————————————
// Middleware
// —————————————————
app.use((0, cors_1.default)({
    credentials: true,
    origin: 'http://localhost:3000', // Allow CORS from local React server
}));
app.use(express_1.default.text()); // Parse incoming requests as plaintext.
// —————————————————
// Serving
// —————————————————
// Serve the static files from the React app.
app.use(express_1.default.static(path.join(__dirname, '..', '..', 'frontend/build')));
// Handles any requests that don't match /api or /static.
app.get(/^\/(?!(api|static)).*/, (req, res) => {
    console.log('°°°°°°°°°');
    res.sendFile(path.join(__dirname, '..', '..', '/frontend/build/index.html'));
});
// —————————————————
// API Routes
// —————————————————
// Endpoints related to keeping score.
app.use('/api/score', score_1.default);
// —————————————————
// Final Middleware
// —————————————————
// Handle errors
app.use((err, req, res, next) => {
    // set locals, only providing error in development
    res.locals.message = err.message;
    res.locals.error = req.app.get('env') === 'development' ? err : {};
    res.status(err.status || res.statusCode || 500);
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
    });
});
// —————————————————
// Create Server
// —————————————————
const http_1 = __importDefault(require("http"));
/**
 * Event handler for server 'error' event.
 */
function onError(err) {
    if (err.syscall !== 'listen') {
        throw err;
    }
    const bind = typeof PORT === 'string' ? 'pipe ' + PORT : 'port ' + PORT;
    // handle specific listen errors with friendly messages
    switch (err.code) {
        case 'EACCES':
            console.error(bind + ' requires elevated privileges');
            process.exit(1);
            break;
        case 'EADDRINUSE':
            console.error(bind + ' is already in use');
            process.exit(1);
            break;
        default:
            throw new Error('oh dear');
    }
}
/**
 * Event listener for server listening event.
 */
const onListening = (server) => () => {
    const addr = server.address();
    const bind = typeof addr === 'string' ? 'pipe ' + addr : 'port ' + addr.port;
    console.log('HTTP: Listening on', bind);
};
/**
 * Builds the server and starts the app.
 */
function start() {
    // Build a server; inject Express app.
    const server = new http_1.default.Server(app);
    server.listen(PORT);
    server.on('error', onError);
    server.on('listening', onListening(server));
}
// —————————————————
// Start
// —————————————————
start();
//# sourceMappingURL=app.js.map
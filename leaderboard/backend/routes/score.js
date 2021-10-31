import express from 'express'

const router = express.Router()

router.get('/', (req, res) => {
    res.json({ msg: '🙆' })
})

router.get('/hit', (req, res) => {
    /**
     * For now, this router will receive data in the format
     * "id={ID}" to indicate that the player of the specified ID was hit.
     */
    console.log(req.query)
    console.log(req.query.id)
    res.send("ok")
})

router.get('/fired', (req, res) => {
    /**
     * For now, this router will receive data in the format
     * "id={ID}" to indicate that the player of the specified ID shot.
     */
    console.log(req.query)
    console.log(req.query.id)
    res.send("ok")
})

export default router

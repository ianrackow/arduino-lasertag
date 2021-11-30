import { it } from "mocha";
// import { server, app } from "../backend/app";
import request from "supertest";

const server = require("../backend/app");

describe("tests", () => {
  it("/GET setState register", (done) => {
    request(server)
      .get("/api/score/setState&state=registration")
      .expect(200, done);
  });
});

// request(app)
//   .get("/")
//   .expect(200)
//   .end((err) => console.log(err));

//   describe("/GET register", () => {
//     it("it should register a client with an id of 1234", () => {
//       chai
//         .request(server)
//         .get("/api/score/register&id=1234")
//         .end((err, res) => {
//           assert.equal(res.status, 200);
//         });
//     });
//   });
// });

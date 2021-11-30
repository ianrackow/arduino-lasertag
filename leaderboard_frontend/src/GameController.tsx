import {
  Button,
  Card,
  CardActions,
  CardContent,
  Grid,
  Typography,
} from "@mui/material";
import { startCase } from "lodash";
import { useState } from "react";

export const GameController = () => {
  enum State {
    Registration = "registration",
    Start = "start",
  }

  const [state, setState] = useState(State.Start);

  return (
    <Card>
      <CardContent>
        <Grid container direction="column" alignItems="center">
          <Grid item>
            <Typography variant="h5" gutterBottom>
              Control Panel
            </Typography>
          </Grid>
          <Grid item>{startCase(state)}</Grid>
        </Grid>
      </CardContent>
      <CardActions>
        {Object.values(State).map((v) => (
          <Button
            disabled={state === v}
            key={v}
            onClick={() => {
              fetch(
                "http://localhost:8888/api/score/setState?" +
                  new URLSearchParams({ state: v })
              )
                .then(() => setState(v))
                .catch((err) => console.log(err));
            }}
          >
            {startCase(v)}
          </Button>
        ))}
      </CardActions>
    </Card>
  );
};

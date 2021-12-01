import {
  Button,
  Card,
  CardActions,
  CardContent,
  Grid,
  Typography,
} from "@mui/material";
import { startCase } from "lodash";
import { Moment } from "moment";
import React, { useState } from "react";
import Countdown from "react-countdown";
import { State } from "./Leaderboard";

export const GameController = ({
  state,
  startTime,
}: {
  state: State;
  startTime: Moment;
}) => {
  const [buttonClicked, setButtonClicked] = useState(false);

  const switchState = (s: State) => {
    setButtonClicked(true);
    fetch(
      "http://localhost:8888/api/score/setState?" +
        new URLSearchParams({ state: s })
    )
      .catch((err) => console.log(err))
      .finally(() => {
        setButtonClicked(false);
      });
  };

  return (
    <Card>
      <CardContent>
        <Grid container direction="column" minWidth={300}>
          <Grid item textAlign="center">
            <Typography variant="h4" gutterBottom>
              Control Panel
            </Typography>
          </Grid>

          <Grid item>
            <Typography variant="overline">
              Current State: {startCase(state)}
            </Typography>
          </Grid>

          {state === State.Registration && (
            <Grid item>
              <Typography variant="caption">
                Accepting all registration requests
              </Typography>
            </Grid>
          )}
          {state === State.Start && (
            <Grid item>
              <Typography variant="caption">
                <Countdown
                  date={startTime.toDate()}
                  renderer={({ seconds, completed }) => {
                    if (completed) {
                      // Render a completed state
                      return "Game On!";
                    } else {
                      // Render a countdown
                      return `Game starting in... ${seconds}`;
                    }
                  }}
                />
                {/* {startTime && startTime.diff(moment(), "seconds") > 0 ? (
                  <>Game beginning in {startTime.diff(moment(), "seconds")}</>
                ) : (
                  <>Game On!</>
                )} */}
              </Typography>
            </Grid>
          )}
        </Grid>
      </CardContent>
      <CardActions>
        {/* {Object.values(State).map((v) => (
          <Button
            disabled={buttonClicked === true}
            key={v}
            onClick={() => {
              
              
            }}
          >
            {startCase(v)}
          </Button>
        ))} */}
        {state !== State.Registration && (
          <Button
            disabled={buttonClicked}
            onClick={() => switchState(State.Registration)}
          >
            Open Registration
          </Button>
        )}
        {state === State.Registration && (
          <Button
            disabled={buttonClicked}
            onClick={() => switchState(State.Start)}
          >
            Start Game
          </Button>
        )}
        {state === State.Start && (
          <Button
            disabled={buttonClicked}
            onClick={() => switchState(State.GameOver)}
          >
            End Game
          </Button>
        )}
      </CardActions>
    </Card>
  );
};

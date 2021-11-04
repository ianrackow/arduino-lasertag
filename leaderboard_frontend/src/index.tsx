import React from "react";
import ReactDOM from "react-dom";
import App from "./App";
import "@fontsource/roboto";
import "@fontsource/roboto/300.css";
import "@fontsource/roboto/400.css";
import "@fontsource/roboto/500.css";
import "@fontsource/roboto/700.css";
import { ThemeProvider } from "@emotion/react";
import { createTheme, CssBaseline, PaletteMode } from "@mui/material";
import { grey } from "@mui/material/colors";

const mode: PaletteMode = "dark" as PaletteMode;

const theme =
  mode === "light"
    ? createTheme({
        palette: {
          mode: mode,
          background: {
            default: grey[50],
          },
        },
      })
    : createTheme({
        palette: {
          mode: mode,
        },
      });

ReactDOM.render(
  <ThemeProvider theme={theme}>
    <CssBaseline />
    <App />
  </ThemeProvider>,
  document.getElementById("root")
);

// If you want to start measuring performance in your app, pass a function
// to log results (for example: reportWebVitals(console.log))
// or send to an analytics endpoint. Learn more: https://bit.ly/CRA-vitals
// reportWebVitals();

import { commonFuncs } from "./lib.js";

renderTemplate(
  "model.h.j2",
  { schemas: schema.components.schemas },
  "model.h",
  commonFuncs
);

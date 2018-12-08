
class Theme{
  color selected    = color(100,200,255);
  color notselected = color(100);
  color background  = color(48);
  int padding = 10; // default padding for all elements. TODO: this was hard coded, needs to be un-hard-codified.
}
Theme theme = new Theme(); // I cannot figure out how to make static classes work without PAapplet complaining.

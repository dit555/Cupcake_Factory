# Automated Cupcake Factory

In this project, I coded a controller for a cupcake assembly line. It works by first placing down a wrapper, filling the wrapper with batter, baking the cupcake, frosting it, decorating it with sprinkles, and then packaging eight cupcakes in a tray.  

This is achieved by first defining a conveyor belt with 8 different sized sections. Six of those sections are for the various cupcake stations and two sections are empty. The conveyor belt layout is a s follows:

|Wrapper|Batter|Empty|Baking|Empty|Frosting|Decorating|Packaging|
|-------|------|-----|------|-----|--------|----------|---------|

The position of the cupcakes by section is shown on output B where B0 is the wrapper section and B7 is the packaging section.  

The wrapper, batter, frosting, decorating, and packaging station are all one foot long. The empty sections are 3 feet long to reduce the chances of heat from the baking section affecting the process of pouring batter and/or frosting the cupcakes. The baking section is 10 feet long. The conveyor belt moves one foot every second. This is too fast for real life purposes, but I choose this to make testing expedited. 

The packager checks that the cupcakes are up to standards and if it is missing anything the cupcake will be discarded and the reason printed to the console. If eight cupcakes pass the check, they are packaged and a message is printed to the console.

The controller also has a safety cutoff switch which shuts down the whole system when toggling it, when the switch is un-toggled production will resume as normal. The switch is mapped to A0, which by default has the system running. The position of the cupcakes by section is shown on B where A0 is the wrapper section and A7 is the packaging section.

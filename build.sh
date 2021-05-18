echo "Running phpize."
if phpize >/dev/null ; then
     echo "Configuring workspace..."
     ./configure >/dev/null
     echo "Done! Please run \"make\" to build extension."
else
     echo "Please install phpize."
fi
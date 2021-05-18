if phpize ; then
     ./configure
     echo "Please run \"make\""
else
     echo "Please install phpize."
fi
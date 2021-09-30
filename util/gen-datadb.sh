#!/bin/sh

# initial empty database "data.db"
#
# copy to folder  /var/lib/radsecproxy

base64 -d - <<END  | bunzip2 > data.db
QlpoOTFBWSZTWcggd0wAAQt/2P/z4IDAbWga+i/8YL+nv0AgAAAgACAASDABEmyEqpsp6mmgGmgA
NAADQB6QANAJJTEyjIZTQAyDQAAAHqAGaQSipoAADQANAAAAMgaBpKgoM4SuToi8SAqYKgw0QnFS
yEJEGzsgtYMTTsyYSQVtpu1dZ9jnT7nwWRyM5mZaVcyrptc2uIYus/nFPVkQh1saE08Mw3ECxBoP
pAYhJ3yEc32cqRdpMkbz8xl5oUNiyWbCxcULOIgHxBIOA4+OzlDDfQbJQ04EIYFCaEgUaLA8OjqK
qqt4hjoh7YEbR+F37i+h73veKBALpFfyUwfsoI+cpkIokI25jKMIzFIjokI9kEhhFlpUQ1DCIud2
aIiIj8QeFJ4yDg8XxkRED+LuSKcKEhkEDumA
END


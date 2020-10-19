
::  btc-bridge.hoon
::  Proxy for accessing BTC full node
::
/-  *btc-bridge, bnh=btc-node-hook
/+  dbug, default-agent, base64, blib=btc-node-json, elib=electrum-rpc-http
|%
+$  versioned-state
    $%  state-0
    ==
::
+$  state-0  [%0 =credentials =status]
::
+$  card  card:agent:gall
::
--
%-  agent:dbug
=|  state-0
=*  state  -
^-  agent:gall
=<
|_  =bowl:gall
+*  this      .
    def   ~(. (default-agent this %|) bowl)
    hc    ~(. +> bowl)
::
++  on-init
  ^-  (quip card _this)
  ~&  >  '%btc-bridge initialized successfully'
  `this(status [%client connected=%.n host=~])
++  on-save
  ^-  vase
  !>(state)
++  on-load
  |=  old-state=vase
  ^-  (quip card _this)
  ~&  >  '%btc-bridge recompiled successfully'
  `this(state !<(versioned-state old-state))
++  on-poke
  |=  [=mark =vase]
  ^-  (quip card _this)
  ::  Only allow poke from our ship, unless we're a host
  ::
  ?>  ?|((team:title our.bowl src.bowl) ?=(%host -.status))
  =^  cards  state
    ?+  mark  (on-poke:def mark vase)
        %btc-bridge-command
      (handle-command:hc !<(command vase))
        %btc-bridge-rpc-action
      (handle-rpc-action !<(rpc-action vase))
    ==
  [cards this]
++  on-watch  on-watch:def
::  TODO: handle /clients path here
::  only allow if we're a host
::  only allow if it's a valid client
::
++  on-leave  on-leave:def
++  on-peek   on-peek:def
++  on-agent  on-agent:def
::  TODO: add our host to status if we're a client when subscription is ack'd here
++  on-arvo
  |=  [=wire =sign-arvo]
  ^-  (quip card _this)
  =*  response  client-response.sign-arvo
  =^  cards  state
  ?+    +<.sign-arvo    (on-arvo:def wire sign-arvo)
      %http-response
    ?+    wire  (on-arvo:def wire sign-arvo)
        [%erpc *]
      ~&  >>>  response
      `state
        [%brpc *]
      (btc-http-response:hc wire response)
    ==
  ==
  [cards this]
::
++  on-fail   on-fail:def
--
::  helper core
|_  =bowl:gall
++  handle-command
  |=  comm=command
  ^-  (quip card _state)
  ?-  -.comm
      %become-host
    `state(credentials credentials.comm, status [%host connected=%.y clients=*(set ship)])
    ::
      %connect-as-client
    :_  state
    ~[[%pass /connect-client %agent [host.comm %btc-provider] %watch /clients]]
    ::
      %allow-clients
    ?+  -.status  ~&(>>> 'Only a %host can add clients' `state)
        %host
      `state(clients.status (~(uni in clients.status) users.comm))
    ==
  ==
++  is-client
  |=  user=ship  ^-  ?
  %.n
++  btc-rpc-auth-header
  =*  user  rpc-user.bc.credentials
  =*  pass  rpc-password.bc.credentials
  :-  'Authorization'
  ;:  (cury cat 3)
  'Basic '
  %-  ~(en base64 | &)
  (as-octs:mimes:html :((cury cat 3) user ':' pass))
  ==
++  btc-gen-request
  |=  req=request:bitcoin-core:rpc
  ^-  request:http
  =*  endpoint  rpc-url.bc.credentials
  =/  body=request:rpc:jstd
    (request-to-rpc:btc-rpc:blib req)
  =/  =header-list:http
    :~  ['Content-Type' 'application/json']
        btc-rpc-auth-header
    ==
  :*  %'POST'
      endpoint
      header-list
      =,  html
      %-  some
      %-  as-octt:mimes
      (en-json (request-to-json:rpc:jstd body))
  ==
::
++  electrum-gen-request
  |=  req=request:electrum:rpc
  %+  request-to-http:electrum-rpc:elib
  rpc-url.ec.credentials  req
++  gen-request
  |=  ract=rpc-action
  ^-  request:http
  ?-  -.ract
      %erpc
    (electrum-gen-request +.ract)
      %brpc
    (btc-gen-request +.ract)
  ==
::
++  handle-rpc-action
  |=  ract=rpc-action
  ^-  (quip card _state)
  =/  out-wire=path
    /[-.ract]/[(scot %da now.bowl)]
  =/  out  *outbound-config:iris
  =/  req=request:http
    (gen-request ract)
  :_  state
  [%pass out-wire %arvo %i %request req out]~
::
++  httr-to-rpc-response
  |=  hit=httr:eyre
  ^-  response:rpc:jstd
  ~|  hit
  =/  jon=json  (need (de-json:html q:(need r.hit)))
  ?.  =(%2 (div p.hit 100))
    (parse-error jon)
  =,  dejs-soft:format
  ^-  response:rpc:jstd
  =;  dere
    =+  res=((ar dere) jon)
    ?~  res  (need (dere jon))
    [%batch u.res]
  |=  jon=json
  ^-  (unit response:rpc:jstd)
  =/  res=[id=(unit @t) res=(unit json) err=(unit json)]
    %.  jon
    =,  dejs:format
    =-  (ou -)
    :~  ['id' (uf ~ (mu so))]
        ['result' (uf ~ (mu same))]
        ['error' (uf ~ (mu same))]
    ==
  ?:  ?=([^ * ~] res)
    `[%result [u.id.res ?~(res.res ~ u.res.res)]]
  ~|  jon
  `(parse-error jon)
::
++  parse-error
  |=  =json
  ^-  response:rpc:jstd
  :-  %error
  ?~  json  ['' '' '']
  %.  json
  =,  dejs:format
  =-  (ou -)
  :~  =-  ['id' (uf '' (cu - (mu so)))]
      |*(a=(unit) ?~(a '' u.a))
      :-  'error'
      =-  (uf ['' ''] -)
      =-  (cu |*(a=(unit) ?~(a ['' ''] u.a)) (mu (ou -)))
      :~  ['code' (uf '' no)]
          ['message' (uf '' so)]
  ==  ==
::
++  btc-http-response
  |=  [=wire response=client-response:iris]
  ^-  (quip card _state)
  ?.  ?=(%finished -.response)
    [~ state]
  =*  status    status-code.response-header.response
  =/  rpc-resp=response:rpc:jstd
    %-  httr-to-rpc-response
    %+  to-httr:iris
      response-header.response
    full-file.response
  ?.  ?=([%result *] rpc-resp)
    ~&  [%error +.rpc-resp]
    [~ state]
  ~&  >  (parse-response:btc-rpc:blib rpc-resp)
  [~ state]
::
--

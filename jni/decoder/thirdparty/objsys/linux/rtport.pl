# port a rt source file to rtx
#
# usage : rtport <file>

if (@ARGV < 1) {
    print "usage: rtport.pl <file>\n";
    exit (-1);
}

open (INFILE, $ARGV[0]) || die ("could not open $ARGV[0]");

$perh = 0;

while (<INFILE>) {
    $perh = 1 if /\bp._common/;

    s/_ASN1([BPX])ER_/_OSRT\1ER_/;

    if (!($_ =~ /ASN1BUF_INDEFLEN/)) {
        s/\bASN1BUF_(\w+)/OSRTBUF\1/g;
    }
    s/\bASN1BUFSAVE/OSRTBufSave/g;
    s/\bASN1MAX\b/OSRTMAX/g;
    s/\bASN1MIN\b/OSRTMIN/g;
    s/ASN1CTXT/OSCTXT/g;
    s/ASN1CtxtPtr/OSRTCtxtPtr/g;
    s/OSContext/OSRTContext/g;
    s/OSCInput/OSRTInput/g;
    s/OSCOutput/OSRTOutput/g;
    s/OSInput/OSRTInput/g;
    s/OSOutput/OSRTOutput/g;
    s/OSFile/OSRTFile/g;
    s/rtErrPrint\s*\(\s*\&ctxt\.errInfo\s*\)/rtxErrPrint (\&ctxt)/g;
    s/rtErr/rtxErr/g;
    s/rtMem/rtxMem/g;
    s/rtCtxt/rtxCtxt/g;
    s/\"rtctype/\"rtxCtype/;
    if (!($_ =~ /rtPrint/ ||
          $_ =~ /rtParse/ ||
          $_ =~ /rtMake/ ||
          $_ =~ /rtBer/ ||
          $_ =~ /rtPer/ ||
          $_ =~ /rtXer/ ||
          $_ =~ /rtXml/ ||
          $_ =~ /rtSax/ ||
          $_ =~ /rtCTo/ ||
          $_ =~ /rtCopy/ ||
          $_ =~ /rtBMP/ ||
          $_ =~ /rtUCS/ ||
          $_ =~ /rtBitStr/ ||
          $_ =~ /rtOctStr/ ||
          $_ =~ /rt\w+OID/ ||
          $_ =~ /rtInitContext/ ||
          $_ =~ /rtInitSubContext/ ||
          $_ =~ /rtFreeContext/)) {
        s/\brt([A-Z])/rtx\1/g;
    }
    s/rtInitContextBuffer/rtxInitContextBuffer/g;
    if (!($_ =~ /rtxsrc/)) {
        if (!($_ =~ /rtkey/)) {
            s/(rtx\w+\.h)/rtxsrc\/\1/;
        }
        if ($_ =~ /rtxsrc\/rtxCpp.+\.h/) {
            s/rtxCpp/OSRT/;
        }
    }
    s/rtInitContext2/rtInitASN1Context/g;
#    s/ALLOC_ASN1ELEM/OSALLOCELEM/g;
    s/EXTERNRT/EXTERNRTX/g;
    s/ASN1UINT64/OSUINT64/g;
    s/ASN1INT64/OSINT64/g;
    s/ASN1UINT8/OSUINT8/g;
    s/ASN1UINT/OSUINT32/g;
    s/ASN1USINT/OSUINT16/g;
    s/ASN1INT8/OSINT8/g;
    s/ASN1INT/OSINT32/g;
    s/ASN1SINT/OSINT16/g;
    s/ASN1OCTET/OSOCTET/g;
    s/ASN1BOOL/OSBOOL/g;
    s/ASN1REAL/OSREAL/g;
    s/ASN1ENUM/OSINT32/g;
    s/ASN1ConstCharPtr/const char*/g;
    s/ASN1ConstOctetPtr/const OSOCTET*/g;
    s/ASN1ConstVoidPtr/const void*/g;
    s/ASN1Const/const/g;
    s/Asn1RTDList/OSRTDList/g;
    s/Asn1RTSList/OSRTSList/g;
    s/ASN1BigInt/OSBigInt/g;
    s/ASN1MemBuf/OSMemBuf/g;
    s/ASN1MEMBUF/OSMEMBUF/g;
    s/\bASN116BITCHAR\b/OSUNICHAR/g;
    s/\bASN132BITCHAR\b/OS32BITCHAR/g;
    s/LOG_ASN1ERR/LOG_RTERR/g;
    s/ASN_E_/RTERR_/g;
    s/ASN_OK_FRAG/RT_OK_FRAG/g;
    s/ASN_OK/0/g;
    s/OSDATETIME/OSNumDateTime/g;
    s/RTERR_INVLEN/ASN_E_INVLEN/g;
    s/RTERR_INVOBJID/ASN_E_INVOBJID/g;
    s/RTERR_BADTAG/ASN_E_BADTAG/g;
    s/RTERR_INVBINS/ASN_E_INVBINS/g;
    s/RTERR_INVINDEX/ASN_E_INVINDEX/g;
    s/RTERR_INVTCVAL/ASN_E_INVTCVAL/g;
    s/RTERR_CONCMODF/ASN_E_CONCMODF/g;
    s/RTERR_ILLSTATE/ASN_E_ILLSTATE/g;
    s/RTERR_RANGERR/RTERR_BADVALUE/g;
    s/ASN1_RESETERR/rtxErrReset/g;
    s/ASN1BUFFER/OSRTBuffer/g;
    s/ASN1MALLOC/rtxMemAlloc/g;
    s/ASN1MEMFREEPTR/rtxMemFreePtr/g;
    s/ASN1MEMFREE/rtxMemFree/g;
    s/ASN1MEMRESET/rtxMemReset/g;
    s/OSREALLOC/rtxMemRealloc/g;
    s/ALLOC_ASN1ELEM/rtxMemAllocType/g;
    if (!($_ =~ /rtsrc/)) {
        s/\"asn1type/\"rtsrc\/asn1type/;
        s/\"asn1intl/\"rtsrc\/asn1intl/;
        s/(asn1Cpp\w+\.h)/rtsrc\/\1/;
        s/(ASN1[CT]\w+\.h)/rtsrc\/\1/;
        s/\"rt_common/\"rtsrc\/rt_common/;
        if ($_ =~ /rtPrint\w*\.h/) {
            s/(rt\w+\.h)/rtsrc\/\1/;
        }
    }
    if (!($_ =~ /rtbersrc/)) {
        s/\"asn1ber/\"rtbersrc\/asn1ber/;
        s/\"x([edu])_common/\"rtbersrc\/x\1_common/;
        s/\"xs([edu])_common/\"rtbersrc\/xs\1_common/;
    }
    if (!($_ =~ /rtpersrc/)) {
        s/\"asn1per/\"rtpersrc\/asn1per/;
        s/\"p([edu])_common/\"rtpersrc\/p\1_common/;
    }
    if (!($_ =~ /rtxersrc/)) {
        s/\"xe([edu])_common/\"rtxersrc\/p\1_common/;
    }
    s/rtxDListAppendNode/rtxDListAppendData/g;
    s/xerState/state/g;
    s/xu_perror/rtxErrPrint/g;
    s/OSBUFFER/OSBuffer/g;
    s/numocts_p/pnumocts/g;
    s/\&pctxt->errInfo/pctxt/g;

    s/RTDIAG(\d)\s*\(/RTDIAG\1 (pctxt, /;

    s/0_FRAG/OSRT_OK_FRAG/g;
    s/\bctxt_p\b/pctxt/g;
    next if /asn1CharSet.h/;
    s/(\w+)->(pSizeConstraint)/ACINFO(\1)->\2/g;
    s/pu_freeContext/rtFreeContext/g;
    s/xu_freeall/rtFreeContext/g;
    
    s/rtxStreamBufClose/rtxStreamClose/g;
    s/rtxStreamBufInit/rtxStreamInit/g;
    s/rtxStreamBufMark/rtxStreamMark/g;
    s/rtxStreamBufReset/rtxStreamReset/g;

    # C++ stream
    s/ASN1BERFileOutputStream\s+out\s+\(filename\)/ASN1BEREncodeStream out (new OSRTFileOutputStream (filename))/;
    s/ASN1BERFileInputStream\s+in\s+\(filename\)/ASN1BERDecodeStream in (new OSRTFileInputStream (filename))/;
    print $_;
    
    #if ($perh == 1 && /#include/ && $incParsed == 0) {
    #   print "#include \"rtxsrc/rtxAsn1Types.h\"\n";   
    #   print "#include \"rtxsrc/rtxDList.h\"\n";   
    #   print "#include \"rtxsrc/rtxCharStr.h\"\n";   
    #   $incParsed = 1;
    #}
}
